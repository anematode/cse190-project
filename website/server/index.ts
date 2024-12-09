import express from 'express'
import * as Path from "path";
import * as fs from "fs";
import {CompileRequest, CompileResponse, ExecutionResponse} from "./api";
const app = express()
const port = 3000

import { spawn } from 'child_process';

import { fileURLToPath } from 'url';
import { dirname } from 'path';
import {resolve} from "node:path";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

app.use(express.json());

app.get('/api', (req, res) => {
    res.send('Hello World!')
});

const CIRCLE_BINARY = Path.join(__dirname, "../vendor/circle");
const TEMP_DIR = Path.join(__dirname, "../temp");
const TEMP_FILE = Path.join(TEMP_DIR, "source.cc");
const EXECUTABLE_FILE = Path.join(TEMP_DIR, "executable");

async function compile(code: string): Promise<{ code: number, stdout: string, stderr: string }> {
    fs.mkdirSync(TEMP_DIR, {recursive: true});
    fs.writeFileSync(TEMP_FILE, code);

    const circleCall = [
        CIRCLE_BINARY, TEMP_FILE, "-o", EXECUTABLE_FILE
    ].join(" ");
    const child = spawn("script", ["-q", "-c", circleCall, "/dev/null", "--return" /* exit code */]);

    let stdout = "", stderr = "";
    child.stdout.on('data', (data: any) => {
        stdout += data;
    });
    child.stderr.on('data', (data: any) => {
        stderr += data;
    });

    return new Promise((resolve, reject) => {
        child.on('close', (code: any) => {
            resolve({ code, stdout, stderr })
        });
    });
}

app.post('/api/compile', (req, res) => {
    const body = req.body as CompileRequest;

    compile(body.code).then((result) => {
        const response: CompileResponse = {
            type: "compile",
            stdout: result.stdout,
            stderr: result.stderr,
            success: result.code === 0
        };

        res.json(response);
    });
});

app.post('/api/run', (req, res) => {
    const body = req.body as CompileRequest;

    compile(body.code).then((result) => {
        if (result.code !== 0) {
            const response: ExecutionResponse = {
                type: "run",
                stdout: "",
                stderr: "Compilation failed",
                success: false
            };
            res.json(response);
            return;
        }

        const MAX_OUTPUT = 100000;
        
        // firejail --quiet --noprofile --private /var/www/circle-huzzah/temp/executable
        const child = spawn("firejail", ["--quiet", "--private", EXECUTABLE_FILE]);
        let stdout = "", stderr = "";
        child.stdout.on('data', (data: any) => {
            stdout += data;
            if (stdout.length > MAX_OUTPUT) {
                kill();
            }
        });
        child.stderr.on('data', (data: any) => {
            stderr += data;
            if (stderr.length > MAX_OUTPUT) {
                kill();
            }
        });

        function kill() {
            child.kill(9);
            killed = true;
        }

        let killed = false;
        child.on('close', (code: any) => {
            const response: ExecutionResponse = {
                type: "run",
                stdout,
                stderr,
                success: code === 0
            };

            if (killed) {
                response.success = false;
                response.stderr = "Execution timed out";
            }

            res.json(response);
        });

        setTimeout(kill, 5000);
    });

});

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
});

import 'monaco-editor/esm/vs/basic-languages/cpp/cpp.contribution';

import MonacoEditor, { monaco } from 'react-monaco-editor';
import React from "react";
import debounce from "lodash.debounce";
import {CompileRequest, CompileResponse, ExecutionResponse} from "../server/api";
import {AnsiUp} from "ansi_up/ansi_up";

const DEFAULT_CODE = `
#feature on safety

#include "../vendor/std2.h"

struct Node {
    int a;
    //std2::optional<std2::box<Node>> next;
};

int main() safe {
  // Objects start off uninitialized. A use of an uninitialized
  // object is ill-formed.
  std2::string s;

  // Require explicit initialization.
  s = std2::string("Hello ");

  // Require explicit mutation.
  mut s.append("World");

  // Require explicit relocation.
  std2::string s2 = rel s;  // Now s is uninitialized.

  // Require explicit non-trivial copy.
  std2::string s3 = cpy s2;

  // \`Hello World\`. 
  println(s3);
}`;

function loadCodeFromLocal() {
    return localStorage.getItem("code") || DEFAULT_CODE;
}

function storeCodeToLocal(code: string) {
    localStorage.setItem("code", code);
}

export function AnsiContainer({ text }: { text: string }) {
    const ansi = new AnsiUp();
    const html = ansi.ansi_to_html(text);

    return <div dangerouslySetInnerHTML={{ __html: html }} />;
}

export type PlaygroundProps = {
    initialCode?: string;
    onChange?: (code: string) => void;
    saveToLocalStorage?: boolean;
}

export function Playground(props: PlaygroundProps) {
    // Create me an app with a monaco editor on the left side, configured to C++, and a compiler output on the right side.

    const [ compilerOutput, setCompilerOutput ] = React.useState<string>("");
    const [ running, setRunning ] = React.useState<boolean>(false);
    const [ code, setCode ] = React.useState<string>(props.saveToLocalStorage ? loadCodeFromLocal : props.initialCode ?? "");
    const [ editor, setEditor ] = React.useState<monaco.editor.IStandaloneCodeEditor | null>(null);

    const [ executionOutput, setExecutionOutput ] = React.useState({ stdout: "", stderr: "" });

    const callback = React.useCallback(debounce(async (newValue: string) => {
        if (props.saveToLocalStorage) storeCodeToLocal(newValue);
        setCode(newValue);
        const request: CompileRequest = {
            type: "compile",
            code: newValue
        };

        const comp: CompileResponse = await (await fetch("/api/compile", {
            method: "POST",
            body: JSON.stringify(request),
            headers: {
                "Content-Type": "application/json"
            }
        })).json();

        setCompilerOutput(comp.stdout);
    }, 500), []);

    const onChange = (value: string) => {
        callback(value);
        if (props.onChange) {
            props.onChange(value);
        }
    }

    const editorElement = <MonacoEditor
        width="50%"
        height="100%"
        language="cpp"
        theme="vs-dark"
        onChange={onChange}
        value={code}
        editorDidMount={(editor) => setEditor(editor)}
        options={{
            automaticLayout: true
        }}
    />;

    async function run() {
        setRunning(true);

        const request: CompileRequest = {
            type: "compile",
            code: code
        };

        const comp: ExecutionResponse = await (await fetch("/api/run", {
            method: "POST",
            body: JSON.stringify(request),
            headers: {
                "Content-Type": "application/json"
            }
        })).json();

        setExecutionOutput({
            stdout: comp.stdout,
            stderr: comp.stderr
        });

        setRunning(false);
    }

    return (
        <div className={"playground"}>
            {editorElement}
            <div className={"outputContainer"}>
            <div className={"compilerOutput"}>
                <button onClick={run} className={"runButton"} disabled={running}>▶ Run</button>

                <AnsiContainer text={compilerOutput} />
            </div>
            <div className={"output"}>
                <div className={"outputHeader"}>Output</div>
                <div className={"outputBody"}>
                    <div className={"outputStdout"}>
                        {executionOutput.stdout}
                    </div>

                    <div className={"outputStderr"}>
                        {executionOutput.stderr}
                    </div>
                </div>
            </div>
            </div>
        </div>
    );
}

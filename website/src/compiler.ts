import {CompileRequest, CompileResponse} from "../server/api";


export async function compileCode(newValue: string) {
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

    return comp;
}
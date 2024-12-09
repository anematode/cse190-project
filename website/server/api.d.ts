
export type CompileRequest = {
    type: "compile",
    code: string
};

export type ExecutionResponse = {
    type: "run",
    stdout: string,
    stderr: string,
    success: boolean,
};

export type CompileResponse = {
    type: "compile",
    stdout: string,
    stderr: string,
    success: boolean,
};

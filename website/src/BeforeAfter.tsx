import React, {useEffect} from 'react';
import MonacoEditor, { monaco } from "react-monaco-editor";
import {MonacoConfig} from "./monaco-config";
import './BeforeAfter.css';
import debounce from "lodash.debounce";
import {compileCode} from "./compiler";
import {AnsiContainer} from "./Playground";
import {calculateHeight} from "./MultipleChoice";

type BeforeAfterProps = {
    before: string;
    after: string;
    afterOriginal: string;
    afterReadonly?: boolean;
    onChange?: (after: string) => void;
    explanation: string;
};

export function BeforeAfter(props: BeforeAfterProps) {
    const [ code, setCode ] = React.useState<string>(props.after);
    const [ editor, setEditor ] = React.useState<monaco.editor.IStandaloneCodeEditor | null>(null);
    const [ compilerOutput, setCompilerOutput ] = React.useState<string>("");

    const compileCallback = React.useCallback(debounce(async (newValue: string) => {
        setCode(newValue);
        const result = await compileCode(newValue);
        setCompilerOutput(result.stdout);
    }, 500), []);

    useEffect(() => {
        compileCallback(props.after);
    }, []);
    
    function onChange(newValue: string) {
        setCode(newValue);
        if (props.onChange) {
            props.onChange(newValue);
        }
        compileCallback(newValue);
    }

    function reset() {
        if (confirm("Are you sure you want to reset this code?") === null) {
            return;
        }
        onChange(props.afterOriginal);
    }

    return (
        <div>
            <h4>{props.explanation}</h4>
            <button onClick={reset}>Reset</button>
            <div className="before-after">
                <div>
                    <h5>Before</h5>
                    <MonacoEditor language={"cpp"} value={props.before} height={calculateHeight(props.before)}
                                  theme={"vs-dark"} options={MonacoConfig}/>
                </div>
                <div>
                    <h5>After</h5>
                    <MonacoEditor language={"cpp"} value={props.after} height={calculateHeight(props.before)}
                                  theme={"vs-dark"} onChange={onChange}
                                  options={{...MonacoConfig, readOnly: !!props.afterReadonly}}/>
                </div>
            </div>

            <h5>Compiler Output</h5>
            <div className="compilerOutput" style={{minHeight: "200px"}}>
                <AnsiContainer text={compilerOutput}/>
            </div>
        </div>
    );
}
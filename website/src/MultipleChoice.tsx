import React from "react";
import MonacoEditor from "react-monaco-editor";
import 'monaco-editor/esm/vs/basic-languages/cpp/cpp.contribution';
import './MultipleChoice.css';
import {MonacoConfig} from "./monaco-config";

type MultipleChoiceProps = {
    selected: number;
    options: (string | { code: string })[];

    question: string;
    code?: string;
    onChange: (selected: number) => void;
}

export function calculateHeight(code: string) {
    return Math.min(200, code.split("\n").length * 19);
}

export function MultipleChoice(props: MultipleChoiceProps) {
    const {selected, options, question, code, onChange} = props;

    return (
        <div>
            <h4 dangerouslySetInnerHTML={{__html: question}}/>
            {code && <MonacoEditor language={"cpp"} value={code} height={calculateHeight(code)} theme={"vs-dark"} options={{
                minimap: {enabled: false},
                automaticLayout: true,
                readOnly: true,
                scrollBeyondLastLine: false,
                scrollbar: {vertical: "hidden", horizontal: "hidden"}
            }}/>}
            <div className={"options-list"}>
                {options.map((option, index) => (
                    <label key={index}>
                        <input
                            type="radio"
                            checked={selected === index}
                            onChange={() => onChange(index)}
                        />
                        {typeof option === "string" ? option : <MonacoEditor language={"cpp"} value={option.code} height={calculateHeight(option.code)} theme={"vs-dark"} options={MonacoConfig}/>}
                    </label>
                ))}
            </div>
        </div>
    );
}
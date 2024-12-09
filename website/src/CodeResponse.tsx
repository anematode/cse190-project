import {Playground} from "./Playground";
import "./MultipleChoice.css";
import {calculateHeight} from "./MultipleChoice";

type CodeResponseProps = {
    initialCode?: string;
}

export function CodeResponse(props: CodeResponseProps) {
    return (
        <div className="coderesponse" style={{height: calculateHeight(props.initialCode ?? ("\n".repeat(50))) + "px"}}>
            <Playground initialCode={props.initialCode} />
        </div>
    );
}
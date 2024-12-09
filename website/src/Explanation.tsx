import * as React from "react";
import "./Explanation.css";

function Element(props: { code: string, explanation: string }) {
    return (
        <div className={"element"}>
            <p style={{fontFamily: "monospace", fontSize: "16px"}}>{props.code}</p>
            <p>–</p>
            <p>{props.explanation}</p>
        </div>
    );
}

export function Explanation() {
    return (
        <div>
            <a href={"https://safecpp.org/draft.html"}>Circle C++ reference</a>

            <h3>Synopsis</h3>
            <div className={"elements"}>
                <Element code={"int^"} explanation={"Mutable borrow of an int"}/>
                <Element code={"const int^"} explanation={"Immutable borrow of an int"}/>
                <Element code={"^var, mut var"} explanation={"Mutable borrow of variable var"}/>
                <Element code={"^const var"} explanation={"Immutable borrow of variable var"}/>
            </div>
        </div>
    )
}
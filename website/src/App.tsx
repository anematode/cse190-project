import React from 'react';
import './App.css';
import {Playground} from "./Playground";
import {MultipleChoice} from "./MultipleChoice";
import {BeforeAfter} from "./BeforeAfter";
import {Explanation} from "./Explanation";

const SAFETY_QUESTIONS = [
    "Yes, it is safe for all inputs.",
    "No, it is not safe for all inputs.",
    "Not sure."
];

type CodeResponseQuestion = {
    type: "coderesponse";
    question: string;
    initialCode: string;
};

type MCQuestion = {
    type: "mc";
    question: string;
    choices: (string | { code: string })[];
    code?: string;
    answer: string;
};

type ExampleRewrite = {
    type: "example-rewrite";
    explanation: string;

    before: string;
    after: string;
};

type Question = MCQuestion | CodeResponseQuestion | ExampleRewrite;

const questions: Question[] = [
    {
        type: "mc",
        question: "Is the following C++ code safe for all input vectors <code>v</code>?",
        choices: SAFETY_QUESTIONS,
        code: `void remove_evens(std::vector<int>& v) {
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        if (*it % 2 == 0) {
            v.erase(it);
            --it;
        }
    }
}`,
        answer: "4"
    },
    {
        type: "mc",
        question: "What is 10 + 10?",
        choices: ["20", "30", "40", "50"],
        answer: "20"
    },
    {
        type: "mc",
        question: "What is 20 + 20?",
        choices: ["40", "50", "60", "70"],
        answer: "40"
    },
    {
        type: "coderesponse",
        question: "Write a function that takes a vector of integers and removes all even numbers.",
        initialCode: `void remove_evens(std::vector<int>& v) {`
    },
    {
        type: "example-rewrite",
        explanation: "This is a good example of a function that removes even numbers from a vector.",
        before: `#include <iostream>

void change_a(int& a) {
\ta = 20;
}

int main() {
\tint a = 10;
\tchange_a(a);
\tstd::cout << a << '\\n';
\treturn 0;
}`,
        after: `#feature on safety
#include "../vendor/std2.h"

void change_a(int& a) safe {
\ta = 20;
}

int main() safe {
\tint a = 10;
\tchange_a(a);
\tstd2::println(a);
\treturn 0;
}`
    }
];

type State = {
    questions: { selected?: number, code?: string }[];
};

function getStateFromLocalStorage(): State {
    const state = localStorage.getItem("state");
    if (state) {
        const parsed = JSON.parse(state);
        if (parsed.questions.length === questions.length) {
            return parsed;
        }
    }

    return {
        questions: questions.map(() => ({ selected: undefined }))
    };
}

function App() {
    const [state, setState] = React.useState<State>(getStateFromLocalStorage);
    const onChange = (index: number, selected: number, code: string) => {
        const newQuestions = state.questions.slice();
        newQuestions[index].selected = selected;
        newQuestions[index].code = code;
        setState({
            questions: newQuestions
        });
    };

    window.onbeforeunload = () => {
        localStorage.setItem("state", JSON.stringify(state));
    };

  return (
    <div className="App glorious">
        <Explanation />
        <h1>Quiz</h1>
        {questions.map((question, index) => {
                switch (question.type) {
                    case "coderesponse":
                        break;
                    case "mc":
                        return <MultipleChoice
                            key={index}
                            question={`${index + 1}. ${question.question}`}
                            options={question.choices}
                            selected={state.questions[index].selected ?? -1}
                            onChange={(selected) => onChange(index, selected, "")}
                            code={question.code}
                        />;
                    case "example-rewrite":
                        return <BeforeAfter before={question.before} after={state.questions[index].code ?? question.after} afterOriginal={question.after} afterReadonly={false} explanation={question.explanation}
                                            onChange={(after: string) => {
                            onChange(index, 0, after);
                        }}></BeforeAfter>
                }
            })}
    </div>
  );
}

export default App;

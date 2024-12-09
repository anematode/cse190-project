import * as monaco from "monaco-editor/esm/vs/editor/editor.api"

export const MonacoConfig: monaco.editor.IStandaloneEditorConstructionOptions = {
    minimap: {enabled: false},
    automaticLayout: true,
    readOnly: true,
    scrollBeyondLastLine: false,
    scrollbar: {vertical: "hidden", horizontal: "hidden"}
};
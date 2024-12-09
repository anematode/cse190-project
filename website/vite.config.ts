import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import viteTsconfigPaths from 'vite-tsconfig-paths';
import {resolve} from 'node:path'

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react({
        jsxRuntime: 'classic'
    }), viteTsconfigPaths()],
    resolve: {
        alias: [{ find: "@", replacement: resolve(__dirname, "./src") }]
    },
    build: {
        rollupOptions: {
            input: {
                main: resolve(__dirname, 'pages/index.html'),
                playground: resolve(__dirname, 'pages/playground/index.html'),
            },
        },
    }
});

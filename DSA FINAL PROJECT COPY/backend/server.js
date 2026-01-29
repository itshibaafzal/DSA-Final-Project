const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const path = require('path');

const app = express();
app.use(cors());
app.use(express.json());

// Spawn C++ Server
// Adjust path if needed. Assuming server.js is in backend/ and ServerMain.exe is in ../
const cppProcess = spawn(path.join(__dirname, '../ServerMain.exe'));

cppProcess.stdout.on('data', (data) => {
    console.log(`C++ Output: ${data}`);
});

cppProcess.stderr.on('data', (data) => {
    console.error(`C++ Error: ${data}`);
});

cppProcess.on('close', (code) => {
    console.log(`C++ process exited with code ${code}`);
});

// Helper to send command to C++
// We might need a queue or promise based approach if we want to wait for response.
// For now, let's keep it simple: One-way trigger or valid response parsing?
// The current C++ loop prints JSON. We need to capture that specific JSON for the response.

let pendingResponse = null;

// Improved C++ reader
let buffer = '';
cppProcess.stdout.on('data', (data) => {
    buffer += data.toString();

    // Check for JSON delimiters
    const startIdx = buffer.indexOf('JSON_START');
    const endIdx = buffer.indexOf('JSON_END');

    if (startIdx !== -1 && endIdx !== -1) {
        const jsonStr = buffer.substring(startIdx + 10, endIdx).trim();
        buffer = buffer.substring(endIdx + 8); // Remove processed part

        try {
            const json = JSON.parse(jsonStr);
            if (pendingResponse) {
                pendingResponse.json(json);
                pendingResponse = null;
            }
        } catch (e) {
            console.error("Failed to parse C++ JSON:", e);
            if (pendingResponse) {
                pendingResponse.status(500).json({ error: "Backend Parse Error" });
                pendingResponse = null;
            }
        }
    }
});

app.post('/api/park', (req, res) => {
    const { plate, type, zone, area } = req.body;
    // PARK <plate> <type> <zone> <area>
    // type: 1=Car, 2=Bike
    const cmd = `PARK ${plate} ${type} ${zone} ${area}\n`;
    console.log("Sending to C++:", cmd.trim());
    cppProcess.stdin.write(cmd);
    pendingResponse = res;
});

app.post('/api/occupy', (req, res) => {
    const { plate, type } = req.body;
    const cmd = `OCCUPY ${plate} ${type}\n`;
    cppProcess.stdin.write(cmd);
    pendingResponse = res;
});

app.post('/api/release', (req, res) => {
    const { plate, type } = req.body;
    const cmd = `RELEASE ${plate} ${type}\n`;
    cppProcess.stdin.write(cmd);
    pendingResponse = res;
});

app.get('/api/status', (req, res) => {
    const cmd = `STATUS\n`;
    cppProcess.stdin.write(cmd);
    pendingResponse = res;
});

app.get('/api/history', (req, res) => {
    const cmd = `HISTORY\n`;
    cppProcess.stdin.write(cmd);
    pendingResponse = res;
});

const PORT = 3001;
app.listen(PORT, () => {
    console.log(`Node Bridge Server running on port ${PORT}`);
});

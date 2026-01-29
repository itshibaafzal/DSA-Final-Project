const axios = require('axios');

async function test() {
    try {
        console.log("Testing PARK operation...");
        const res = await axios.post('http://localhost:3001/api/park', {
            plate: "TEST-01",
            type: 1,
            zone: 1,
            area: 0
        });
        console.log("PARK Response:", res.data);

        if (res.data.result === 'success') {
            console.log("✅ PARK Passed");
        } else {
            console.log("❌ PARK Failed");
        }

        console.log("\nTesting OCCUPY operation...");
        const res2 = await axios.post('http://localhost:3001/api/occupy', {
            plate: "TEST-01",
            type: 1
        });
        console.log("OCCUPY Response:", res2.data);

    } catch (e) {
        console.error("Test Failed:", e.message);
    }
}

test();

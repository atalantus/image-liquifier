const wasmPath = "./liquifier.wasm";

const canvas = document.getElementById("imageCanvas");
const context = canvas.getContext("2d");

const importObject = {
    "env" : {
        "sinf": (x) => Math.sin(x),
        "cosf": (x) => Math.cos(x),
        "sqrtf": (x) => Math.sqrt(x)
    }
};

const w = await WebAssembly.instantiateStreaming(fetch(wasmPath), importObject);

/**
 * Start liquifying the image
 */
async function liquify() {

    /**
     * Render the current simulation state as canvas frame
     */
    function renderFrame(dt) {

    }

    /**
     * Update canvas in loop
     */
    let lastUpdate = null;
    function updateCanvas(timestamp) {
        const dt = timestamp - lastUpdate;
        lastUpdate = timestamp;
        renderFrame(dt);
        window.requestAnimationFrame(updateCanvas);
    }

    window.requestAnimationFrame((timestamp) => {
        lastUpdate = timestamp;
        renderFrame(0);
        window.requestAnimationFrame(updateCanvas);
    });
}
import ModuleEngine from "./engine.js";
import { Texture2D, Color } from "./structs.js";

let Module;

let load_asset_memory;
let load_texture;
let init_window;
let begin_drawing;
let clear_background;
let draw_rectangle;
let draw_texture;
let end_drawing;
let init;


async function boot(assetsPreload = []) {
    if (!Module) {
        Module = await ModuleEngine({
            canvas: document.querySelector("#canvas"),
        });
        console.log(Module);
        console.log(Module.FS);

        load_asset_memory = Module.cwrap('load_asset_memory', null, ['number', 'number', 'string']);
        load_texture = Module.cwrap('load_texture', 'number', ['string']);
        init_window = Module.cwrap('init_window', null, ['number', 'number', 'string']);
        begin_drawing = Module.cwrap('begin_drawing', null, null);
        clear_background = Module.cwrap('clear_background', null, ['number', 'number', 'number', 'number']);
        draw_rectangle = Module.cwrap('draw_rectangle', null, ['number', 'number', 'number', 'number']);
        draw_texture = Module.cwrap('draw_texture', null, ['number', 'number', 'number']);
        end_drawing = Module.cwrap('end_drawing', null, null);
        init = Module.cwrap('init', null, null);

        init();

        let promisses = [];
        assetsPreload.forEach(element => {
            promisses.push(Promise.resolve(loadAssetMemory(element)));
        });


        await Promise.all(promisses).then((values) => {
            console.log(values);
        });

        return Module;
    }
}

async function loadAssetMemory(fileName) {

    try {

        let fileNameOnly = fileName;
        fileNameOnly = fileNameOnly.split('\\').pop().split('/').pop();

        console.log(fileNameOnly);

        const response = await fetch('/' + fileName); // Certifique-se de que o caminho para o arquivo está correto
        if (response.status !== 200) throw new Error(response.url);

        const arrayBuffer = await response.arrayBuffer();
        const fileData = new Uint8Array(arrayBuffer);

        // Verifique se o Module e _malloc estão disponíveis
        if (!Module || typeof Module._malloc !== 'function') {
            throw new Error('Module._malloc não está disponível');
        }


        let dataPtr = Module._malloc(fileData.length); // Alocando memória
        Module.HEAPU8.set(fileData, dataPtr); // Copiando os valores para a memória alocada
        load_asset_memory(dataPtr, fileData.length, fileNameOnly);
        Module._free(dataPtr); // Liberando a memória alocada

        console.log('carregou e liberou memoria');
        return fileNameOnly;

    } catch (error) {
        console.error('Erro ao carregar o arquivo:', error);
    }

}



export function initWindow(width = 800, height = 600, title = "web") {
    init_window(width, height, title);
}

export function loadTexture(fileName) {

    const texturePtr = load_texture(fileName); // Chamar a função e obter o ponteiro para a struct        
    const texture = new Texture2D(  // Ler os valores da struct
        Module.getValue(texturePtr, 'i32'),
        Module.getValue(texturePtr + 4, 'i32'),
        Module.getValue(texturePtr + 8, 'i32'),
        Module.getValue(texturePtr + 12, 'i32'),
        Module.getValue(texturePtr + 16, 'i32')
    );

    return texture;
}

export function drawRectangle(x, y, width, height) {
    draw_rectangle(x, y, width, height);
}

export function drawTexture(texture, posX, posY) {

    // Alocando memória para a struct Texture2D
    let texturePtr = Module._malloc(20); // 5 ints * 4 bytes cada


    Module.setValue(texturePtr, texture.id, 'i32');  // Copiando os valores da struct para a memória alocada
    Module.setValue(texturePtr + 4, texture.width, 'i32');
    Module.setValue(texturePtr + 8, texture.height, 'i32');
    Module.setValue(texturePtr + 12, texture.mipmaps, 'i32');
    Module.setValue(texturePtr + 16, texture.format, 'i32');

    draw_texture(texturePtr, posX, posY);

    // Liberando a memória alocada
    Module._free(texturePtr);
}

export function clearBackground(color = Color.raywhite) {
    clear_background(color.r, color.g, color.b, color.a);
}

export function beginDrawing() {
    begin_drawing();
}

export function endDrawing(params) {
    end_drawing();
}

export var Raylib = boot;
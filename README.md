<p align="center">
  <img src="media/logo.png" alt="App logo" width="96">
</p>

<h1 align="center">
  Particle System JS
</h1>

<p align="center">
  Port of a C++ Program to JS
</p>

## Table Of Contents

- [Features](#features)
- [Try It Online](#try-it-online)
- [Port Source](#port-source)
- [Technology Stack](#technology-stack)
- [How To Start](#how-to-start)
- [Demo](#demo)
- [License](#license)

## Features

### Control

- **W** - toggle ui

### Program windows

- **Windows** - window management
- **Scene selector** - scene change
- **Scene variables** - changing the parameters of objects on the scene
- **Environment** - information about computer
- **Performance** - statistics

## Try It Online

At GitHub: https://trequend.github.io/particle-system-js

## Port Source

At GitHub: https://github.com/Trequend/ParticleSystem

## Technology Stack

- Emscripten
- Webpack
- OpenGL 3.0 ES (WebGL 2)
- GLEW
- GLFW
- GLM
- Dear ImGui

## How To Start

Recommended editor: `Visual Studio Code`

Required tools:

- Git
- Node.js (>= 16.13.0)
- Yarn (>= 1.22.17)
- CMake (>= 3.23.0)
- Ninja (>= 1.10.2)
- Emscripten SDK (emsdk >= 3.1.8)

Download project:

```bash
git clone https://github.com/trequend/particle-system-js.git
cd particle-system-js
```

Install dependencies:

```bash
yarn install
```

### Development

Start application:

```bash
yarn run dev
```

Compile cpp (run after cpp code change):

```bash
yarn run rebuild-cpp
```

### Build

```bash
yarn run build
```

## Demo

![Demo](media/demo.gif)

## License

MIT

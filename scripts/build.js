const fs = require('fs/promises');
const { existsSync } = require('fs');
const path = require('path');
const { spawn } = require('child_process');

const isProduction = process.env.NODE_ENV === 'production';
const mode = isProduction ? 'Release' : 'Debug';

const BUILD_FOLDER = path.resolve(__dirname, '../build');
const OUTPUT_FOLDER = path.join(BUILD_FOLDER, mode);
const BIN_FOLDER = path.resolve(__dirname, '../src/particle-system/bin');

const cmake = createCommand('cmake', ['--version']);
const emcmake = createCommand(isWindows() ? 'emcmake.bat' : 'emcmake', [cmake.commandName, '--help']);

(async function build() {
  if (!(await cmake.exists())) {
    console.error('Cmake not found. Install cmake.');
    process.exit(1);
  }

  if (!(await emcmake.exists())) {
    console.error('Emcmake not found. Install emscripten.');
    process.exit(1);
  }

  await createBuildFolder();
  await emcmake([cmake.commandName, '..', '-G', 'Ninja Multi-Config'], { cwd: BUILD_FOLDER });
  await cmake(['--build', '.', '--config', mode], { cwd: BUILD_FOLDER });
  await createBinFolder();
  await forEachFile(OUTPUT_FOLDER, (name, filePath) => {
    fs.copyFile(filePath, path.join(BIN_FOLDER, name));
  });
})();

async function createBuildFolder() {
  if (existsSync(BUILD_FOLDER)) {
    await fs.rm(BUILD_FOLDER, { recursive: true });
  }

  await fs.mkdir(BUILD_FOLDER);
}

async function createBinFolder() {
  if (existsSync(BIN_FOLDER)) {
    await fs.rm(BIN_FOLDER, { recursive: true });
  }

  await fs.mkdir(BIN_FOLDER);
}

async function forEachFile(folder, callback) {
  if (!existsSync(folder)) {
    return;
  }

  const names = await fs.readdir(folder);
  for (const name of names) {
    const filePath = path.join(folder, name);
    const stat = await fs.stat(filePath);
    if (stat.isFile()) {
      await callback(name, filePath);
    }
  }
}

function createCommand(name, checkArgs = []) {
  async function command(args = [], { cwd, verbose }) {
    verbose = verbose === undefined ? true : false;
    return new Promise((resolve, reject) => {
      const child = spawn(name, args, {
        stdio: ['ignore', verbose ? 'inherit' : 'ignore', verbose ? 'inherit' : 'ignore'],
        cwd,
      });

      child.on('close', (code) => {
        if (code === 0) {
          resolve();
        } else {
          reject(new Error(`${name} error code: ${code}`));
        }
      });

      child.on('error', (error) => {
        reject(error);
      });
    });
  }

  command.exists = async () => {
    try {
      await command(checkArgs, { verbose: false });
      return true;
    } catch {
      return false;
    }
  };

  command.commandName = name;

  return command;
}

function isWindows() {
  return process && (process.platform === 'win32' || /^(msys|cygwin)$/.test(process.env.OSTYPE));
}

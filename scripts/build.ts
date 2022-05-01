import fs from 'fs/promises';
import path from 'path';
import { args } from './src/args';
import { cmake, emcmake } from './src/commands';
import { env } from './src/env';
import { binFolder, buildFolder, outputFolder } from './src/folders';

(async function build() {
  await cmake.throwIfNotExists();
  await emcmake.throwIfNotExists();

  if (!buildFolder.exists()) {
    await buildFolder.create();
    await emcmake.invoke([cmake.name, '..', '-G', 'Ninja Multi-Config'], { cwd: buildFolder.path });
  }

  await cmake.invoke(['--build', '.', '--config', env.mode, ...args], { cwd: buildFolder.path });
  await binFolder.create({ clear: true });
  await outputFolder.forEachFile(async (name, filePath) => {
    await fs.copyFile(filePath, path.join(binFolder.path, name));
  });
})();

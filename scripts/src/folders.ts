import path from 'path';
import { env } from './env';
import { Folder } from './folder.class';

export const buildFolder = new Folder(path.resolve(__dirname, '../../build'));
export const outputFolder = new Folder(path.join(buildFolder.path, env.mode));
export const binFolder = new Folder(path.resolve(__dirname, '../../src/particle-system/bin'));

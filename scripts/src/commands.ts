import { Command } from './command.class';
import { isWindows } from './is-windows';

export const cmake = new Command('cmake', {
  checkArgs: ['--version'],
  notFoundMessage: 'Cmake not found. Please install Cmake.',
});

export const emcmake = new Command(isWindows() ? 'emcmake.bat' : 'emcmake', {
  checkArgs: [cmake.name, '--help'],
  notFoundMessage: 'Emcmake not found. Plese install Emsdk.',
});

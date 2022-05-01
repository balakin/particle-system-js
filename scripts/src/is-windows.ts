export function isWindows() {
  return process && (process.platform === 'win32' || /^(msys|cygwin)$/.test(process.env.OSTYPE || ''));
}

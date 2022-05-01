import { existsSync } from 'fs';
import fs from 'fs/promises';
import path from 'path';

export class Folder {
  constructor(public readonly path: string) {}

  async create({ clear }: CreateOptions = {}) {
    if (this.exists()) {
      if (clear) {
        await fs.rm(this.path, { recursive: true });
      } else {
        return;
      }
    }

    await fs.mkdir(this.path);
  }

  exists() {
    return existsSync(this.path);
  }

  async forEachFile(callback: ForEachCallback) {
    if (!this.exists()) {
      throw new Error('Folder not exists');
    }

    const names = await fs.readdir(this.path);
    for (const name of names) {
      const filePath = path.join(this.path, name);
      const stat = await fs.stat(filePath);
      if (stat.isFile()) {
        await callback(name, filePath);
      }
    }
  }
}

interface CreateOptions {
  clear?: boolean;
}

interface ForEachCallback {
  (name: string, filePath: string): Promise<void> | void;
}

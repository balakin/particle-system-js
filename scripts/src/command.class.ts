import { spawn } from 'child_process';

export class Command {
  private readonly checkArgs: string[];
  private readonly notFoundMessage: string;

  constructor(public readonly name: string, { checkArgs, notFoundMessage }: CommandOptions) {
    this.notFoundMessage = notFoundMessage;
    this.checkArgs = checkArgs;
  }

  async exists() {
    try {
      await this.invoke(this.checkArgs, { verbose: false });
      return true;
    } catch {
      return false;
    }
  }

  async throwIfNotExists() {
    if (await this.exists()) {
      return;
    }

    throw new Error(this.notFoundMessage);
  }

  async invoke(args?: string[], options?: InvokeOptions) {
    let { cwd, verbose } = options || {};
    verbose = verbose === undefined ? true : false;

    return new Promise<void>((resolve, reject) => {
      const child = spawn(this.name, args === undefined ? [] : args, {
        stdio: ['ignore', verbose ? 'inherit' : 'ignore', verbose ? 'inherit' : 'ignore'],
        cwd,
      });

      child.on('close', (code) => {
        if (code === 0) {
          resolve();
        } else {
          reject(new Error(`${this.name} error code: ${code}`));
        }
      });

      child.on('error', (error) => {
        reject(error);
      });
    });
  }
}

interface CommandOptions {
  checkArgs: string[];
  notFoundMessage: string;
}

interface InvokeOptions {
  cwd?: string;
  verbose?: boolean;
}

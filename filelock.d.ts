export = FileLock;
export declare interface FileLock {
  lock: () => boolean;
  unlock: () => boolean;
  isLocked: () => boolean;
  getFilePath: () => string | undefined;
  setFilePath: (filePath: string) => boolean;
}
export class FileLock {
  constructor(filePath: string);
  fileLock: any;
  lock: () => boolean;
  unlock: () => boolean;
  isLocked: () => boolean;
  getFilePath: () => string | undefined;
  setFilePath: (filePath: string) => boolean;
}

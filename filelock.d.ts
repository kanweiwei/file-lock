export = FileLock;
declare function FileLock(filePath: string): void;
export declare interface FileLock {
  lock: () => boolean;
  unlock: () => boolean;
  isLocked: () => boolean;
  getFilePath: () => void;
  setFilePath: (filePath: string) => boolean;
}
declare class FileLock {
  constructor(filePath: string): void;
  fileLock: any;
  lock: () => boolean;
  unlock: () => boolean;
  isLocked: () => boolean;
  getFilePath: () => void;
  setFilePath: (filePath: string) => boolean;
}

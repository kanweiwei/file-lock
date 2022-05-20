const fileLock = require("bindings")({
  try: [
    [
      "module_root",
      "node_modules",
      "@nshq/file-lock",
      "build",
      "Release",
      "bindings",
    ],
    [
      "module_root",
      "build",
      "Release",
      "bindings",
    ],
  ],
  bindings: "file_lock",
}).FileLock;
class FileLock {
  constructor(filePath) {
    this.fileLock = new fileLock(filePath);
  }

  lock() {
    return this.fileLock.lock();
  }

  unlock() {
    return this.fileLock.unlock();
  }

  isLocked() {
    return this.fileLock.isLocked();
  }

  getFilePath() {
    return this.fileLock.getFilePath();
  }
}

module.exports = FileLock;

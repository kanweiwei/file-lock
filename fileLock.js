const fileLock = require("bindings")("fileLock.node").FileLock;

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

  getFilePath() {
    return this.fileLock.getFilePath();
  }
}

module.exports = FileLock;

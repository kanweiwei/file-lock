var fileLock = require("bindings")({
  try: [
    [
      "module_root",
      "node_modules",
      "@nshq/file-lock",
      "build",
      "Release",
      "bindings",
    ],
    ["module_root", "build", "Release", "bindings"],
  ],
  bindings: "file_lock",
}).FileLock;

function FileLock(filePath) {
  this.fileLock = new fileLock(filePath);
}

FileLock.prototype.lock = function () {
  return this.fileLock.lock();
};

FileLock.prototype.unlock = function () {
  return this.fileLock.unlock();
};

FileLock.prototype.isLocked = function () {
  return this.fileLock.isLocked();
};

FileLock.prototype.getFilePath = function () {
  return this.fileLock.getFilePath();
};

FileLock.prototype.setFilePath = function (filePath) {
  if (this.fileLock.isLocked()) {
    throw new Error("File is locked");
  }
  return this.fileLock.setFilePath(filePath);
};

module.exports = FileLock;

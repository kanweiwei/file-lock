var binary = require("@mapbox/node-pre-gyp");
var path = require("path");
var binding_path = binary.find(
  path.resolve(path.join(__dirname, "../package.json"))
);
var binding = require(binding_path);
var fileLock = binding.FileLock;

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

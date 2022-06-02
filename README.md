# file-lock

使用 node-addon-api 开发，用于独占打开文件。

# Support

| os      | support |
| ------- | ------- |
| windows | ✅      |
| mac     | ✅      |
| linux   | ❔      |

# Install Package

```bash
yarn add @nshq/file-lock
```

# Usage

```javascript
const FileLock = require("@nshq/file-lock");

const filePath = "/tmp/xxx";
const fileLock = new FileLock(filePath);

fileLock.lock(); // boolean
fileLock.isLocked(); // true
//
fileLock.unlock();
fileLock.isLocked(); // false
```

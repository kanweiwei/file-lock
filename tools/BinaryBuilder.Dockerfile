ARG NODE_VERSION=16
ARG VARIANT=bullseye

FROM node:$NODE_VERSION-$VARIANT

ARG VARIANT

RUN if [ "$VARIANT" = "alpine" ] ; then apk add build-base python3 --update-cache ; fi

WORKDIR /usr/src/build

COPY . .
RUN npm install --ignore-scripts

# Workaround for https://github.com/mapbox/node-pre-gyp/issues/644
RUN cd node_modules/\@mapbox/node-pre-gyp \
  && npm install fs-extra@10.0.1 \
  && sed -i -e s/\'fs/\'fs-extra/ -e s/fs\.renameSync/fs.moveSync/ ./lib/util/napi.js

RUN npx node-pre-gyp configure
RUN npx node-pre-gyp build

RUN if [ "$VARIANT" != "alpine" ] ; then ldd lib/binding/*/file-lock.node; nm lib/binding/*/file-lock.node | grep "GLIBC_" | c++filt || true ; fi

RUN npx node-pre-gyp package

CMD ["sh"]
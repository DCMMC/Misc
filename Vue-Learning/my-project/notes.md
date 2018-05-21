## Vue + Vue-Material 安装笔记

```
# 最新稳定版
$ npm install vue
```

跑 demo:

```
# 全局安装 vue-cli
$ npm install --global vue-cli
# 创建一个基于 webpack 模板的新项目
$ vue init webpack my-project
# 安装依赖，走你
$ cd my-project
$ npm run dev
```

安装 Vue-Material(记得这些安装都是必须在项目根目录下才能调用):

```
$ npm install vue-material@beta --save
$ npm install sass-loader node-sass --save-dev
```

因为默认我在 vue-cli 创建项目的时候选择了 code style checker, 所以必须按照规范, 不然没法运行.

## Vue 项目目录结构

根目录下的 config 里面是配置文件, index.html 是主 html 文件, src 里面的 App.vue 和其他文件夹下的 *.vue 都是在 main.js 中被导入的, 在 main.js 中放主 js 脚本, 也就是 Vue 的实例就在这里创建.

[scss import not found 的问题](https://stackoverflow.com/questions/41368263/how-to-import-external-scss-properly-with-webpack-and-vue-js)

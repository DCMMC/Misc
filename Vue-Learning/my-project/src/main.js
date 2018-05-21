// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import VueMaterial from 'vue-material'
// import 'babel'
import 'vue-material/dist/vue-material.min.css'
import 'vue-material/dist/theme/engine.scss'
import 'vue-material/dist/theme/default.css'

import App from './App'

Vue.use(VueMaterial)

const app = new Vue({
  name: 'Demo',
  components: { App },
  template: '<App></App>',
  data: function data () {
    return {
      theme: 'teal'
    }
  }
})

app.$mount('#app')

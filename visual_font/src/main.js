import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import iView from 'iview';
import 'iview/dist/styles/iview.css';
import ECharts from 'vue-echarts'

Vue.config.productionTip = false
Vue.use(iView);
// register component to use
new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')

Vue.component('v-chart', ECharts)
router.push({name:'table_show'})
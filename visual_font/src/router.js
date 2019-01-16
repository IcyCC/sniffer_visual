import Vue from 'vue'
import Router from 'vue-router'
import Home from './views/Home.vue'

Vue.use(Router)

export default new Router({
    routes: [
        {
            path: '/',
            name: 'home',
            component: Home,
            children: [
                {
                    path: '/frequency',
                    name: 'frequency',
                    component: () => import('@/views/FrequencyChart')
                },
                {
                    path: '/table_show',
                    name: 'table_show',
                    component: () => import('@/views/TableShow')
                },
                {
                    path: '/hot_point',
                    name: 'hot_point',
                    component: () => import('@/views/HotPointClient')
                }
            ]
        },
    ]
})
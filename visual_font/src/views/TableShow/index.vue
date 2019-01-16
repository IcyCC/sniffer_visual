<template>
    <Card>
        <Table border stripe :columns="columns" :data="data"></Table>
        <div style="margin: 10px;overflow: hidden">
            <div style="float: right;">
                <Page :total="total" show-total :page-size="pages._per_page" :current="pages._page"
                      @on-change="onPageChange"></Page>
            </div>
        </div>
    </Card>
</template>
<script>
    import {queryRequestInfos} from 'API'
    import request_detail from './components/request_detail'
    export default {
        name: "TableShow",
        components:{request_detail},
        data: function () {
            return {
                pages:{
                  _per_page:30,
                    _page: 1
                },
                columns: [
                    {
                        type: 'expand',
                        title:"评价",
                        width: 70,
                        render: (h, params) => {
                            return h(request_detail, {
                                props: {
                                    request_id: params.row.id
                                }
                            })
                        }
                    },
                    {
                        title: "编号",
                        width: 100,
                        render: function (h, params) {
                            return h('span', {
                            }, params.row.id)
                        }
                    },
                    {
                        title: "时间",
                        render: function (h, params) {
                            return h('span', {

                            }, params.row.created_at)
                        }
                    },
                    {
                        title: "源地址",
                        render: function (h, params) {
                            return h('span', {

                            }, params.row.src)
                        }
                    },
                    {
                        title: "目的地址",
                        render: function (h, params) {
                            return h('span', {

                            }, params.row.dest)
                        }
                    },
                ],
                data:[],
                total: 0
            }
        },
        methods:{
            fetch: function () {
                return queryRequestInfos({...this.pages}).then((resp)=>{
                    this.data = resp.data.request_infos
                    this.total = resp.data.total
                })
            },
            onPageChange: function (page) {
                this.pages._page = page
                this.fetch()
            }
        },
        mounted:function () {
            this.fetch()
        }
    }
</script>

<style scoped>

</style>
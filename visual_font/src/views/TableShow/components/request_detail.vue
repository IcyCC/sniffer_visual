<template>
    <card>
        <Table :stripe="true" :border="false" :data="request_headers" :columns="columns"></Table>
    </card>
</template>

<script>
    import {queryRequestHeaders} from 'API'
    export default {
        name: "request_detail",
        props: {
            request_id: Number,
        },
        data: function(){
            return {
                request_headers:[],
                columns: [

                    {
                        title: 'key',
                        render: function (h, params) {
                            return h('span', {
                            }, params.row.header_key)
                        }
                    },
                    {
                        title: 'value',
                        render: function (h, params) {
                            return h('span', {
                            }, params.row.header_value)
                        }
                    }]
            }
        },
        mounted:function () {
            queryRequestHeaders({request_id: this.request_id}).then((resp)=>{
                this.request_headers = resp.data.request_headers
            })
        }
    }
</script>

<style scoped>

</style>
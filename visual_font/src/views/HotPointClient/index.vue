<template>
    <div>
        <Card>
            <Row>
                <Col span="6">
                    <Select filterable
                            clearable
                            @on-query-change="handleSrcQueryChange"
                            :value="this.select_src"
                            @on-change="handleSelectSrcChange"
                            style="width: 280px; float: left">
                        <Option v-for="item in src_list" :value="item.src" :key="item.src">{{ item.src }}</Option>
                    </Select>
                </Col>
            </Row>
        </Card>
        <div style="padding-top: 40px"></div>
        <card>
            <div v-if="client_host.length" style="height: 450px">
                <Pie title="请求分布" :series="client_host"></Pie>
            </div>
            <div v-else>
                <span>暂无数据</span>
            </div>
        </card>
    </div>
</template>

<script>
    import Pie from '@/components/pie'
    import {queryClientSrcLike,queryCLientHostRanks} from 'API'

    export default {
        components: {Pie},
        name: "HotPoint",
        data: function () {
            return {
                select_src: '',
                src_like: '',
                src_list: [],
                client_host: []
            }
        },
        methods: {
            fetchClientSrcLike: function () {
                return queryClientSrcLike(this.src_like).then((resp) => {
                    this.src_list = resp.data.client_srcs
                })
            },
            fetchClientHostRanks: function(){
                return queryCLientHostRanks(this.select_src).then((resp)=>{
                    this.client_host = resp.data.client_hosts.map((item)=>{
                        return {
                            name: item.header_value,
                            value: item.num
                        }
                    })
                })
            },
            handleSrcQueryChange: function (src_like) {
                this.src_like = src_like
                this.fetchClientSrcLike()
            },
            handleSelectSrcChange: function (v) {
                this.select_src = v
                this.fetchClientHostRanks()
            },
        },
        mounted: function () {
            this.handleSrcQueryChange()
        }
    }
</script>

<style scoped>

</style>
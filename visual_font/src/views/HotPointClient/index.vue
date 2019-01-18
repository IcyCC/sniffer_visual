<template>
    <div>
        <Card>
            <Row>
                <Col span="8">
                            <span>
                                客户端:
                       <Select filterable
                                   clearable
                                   @on-query-change="handleSrcQueryChange"
                                   :value="this.select_src"
                                   @on-change="handleSelectSrcChange"
                               @on-clear="handleSelectSrcClear"
                               style="width: 260px">
                        <Option v-for="item in src_list" :value="item.src" :key="item.src">{{ item.src }}</Option>
                    </Select>
                            </span>
                </Col>
            </Row>
        </Card>
        <div style="padding-top: 40px"></div>
        <card>
            <Tabs v-model="select_tab">
                <TabPane label="访问热点图" name="hot" >
                </TabPane>
                <TabPane label="访问时间图" name="time">

                </TabPane>
            </Tabs>
            <div>
                <div v-if="select_tab === 'hot' ">
                    <div v-if="client_host.length" style="height: 450px">
                        <Pie title="请求分布" :series="client_host"></Pie>
                    </div>
                    <div v-else>
                        <span>暂无数据</span>
                    </div>
                </div>
                <div v-else-if="select_tab === 'time'">
                    <div v-if="Object.keys(line_data).length" style="height: 450px">
                        <LineT :series="line_data" title="请求时间"></LineT>
                    </div>
                    <div v-else>
                        <span>暂无数据</span>
                    </div>
                </div>
            </div>
        </card>
    </div>
</template>

<script>
    import Pie from '@/components/pie'
    import LineT from '../../components/time_line'
    import {queryClientSrcLike,queryCLientHostRanks,queryClientHourTime} from 'API'

    export default {
        components: {Pie,LineT},
        name: "HotPoint",
        data: function () {
            return {
                select_src: '',
                src_like: '',
                src_list: [],
                client_host: [],
                select_tab: 'hot_host',
                line_data:{}
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
                            name: item.host,
                            value: item.num
                        }
                    })
                })
            },
            fetchClientHourTime: function(){
                return queryClientHourTime(this.select_src).then((resp)=>{
                    this.line_data = {}
                    this.line_data[this.select_src] = resp.data.client_time.map((item)=>{
                        return [item.time, item.num]
                    })
                })
            },
            handleSrcQueryChange: function (src_like) {
                this.src_like = src_like
                this.fetchClientSrcLike()
            },
            handleSelectSrcClear:function(){
                for(let key in this.line_data){
                    delete this.line_data[key]
                }
                this.client_host= []
            },
            handleSelectSrcChange: function (v) {
                this.select_src = v
                this.fetchClientHostRanks()
                this.fetchClientHourTime()
            },
        },
        mounted: function () {
            this.handleSrcQueryChange()
            this.select_tab = 'hot'
        }
    }
</script>

<style scoped>

</style>
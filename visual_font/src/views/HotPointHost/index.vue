<template>
    <div>
        <Card>
            <Row>
                <Col span="8">
                            <span>
                                服务端:
                       <Select filterable
                                   clearable
                                   @on-query-change="handleSrcQueryChange"
                                   :value="this.select_host"
                                   @on-change="handleSelectSrcChange"
                               @on-clear="handleSelectSrcClear"
                               style="width: 260px">
                        <Option v-for="item in host_list" :value="item.host" :key="item.host">{{ item.host }}</Option>
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
                    <div v-if="host_client.length" style="height: 450px">
                        <Pie title="请求分布" :series="host_client"></Pie>
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
    import {queryHostSrcLike,queryHostCLientRanks,queryHostHourTime} from 'API'

    export default {
        components: {Pie,LineT},
        name: "HotPoint",
        data: function () {
            return {
                select_host: '',
                host_like: '',
                host_list: [],
                host_client: [],
                select_tab: 'hot',
                line_data:{}
            }
        },
        methods: {
            fetchClientSrcLike: function () {
                return queryHostSrcLike(this.host_like).then((resp) => {
                    this.host_list = resp.data.host_srcs
                })
            },
            fetchClientHostRanks: function(){
                return queryHostCLientRanks(this.select_host).then((resp)=>{
                    this.host_client = resp.data.host_clients.map((item)=>{
                        return {
                            name: item.src,
                            value: item.num
                        }
                    })
                })
            },
            fetchClientHourTime: function(){
                return queryHostHourTime(this.select_host).then((resp)=>{
                    this.line_data = {}
                    this.line_data[this.select_host] = resp.data.host_time.map((item)=>{
                        return [item.time, item.num]
                    })
                })
            },
            handleSrcQueryChange: function (host_like) {
                this.host_like = host_like
                this.fetchClientSrcLike()
            },
            handleSelectSrcClear:function(){
                for(let key in this.line_data){
                    delete this.line_data[key]
                }
                this.host_client= []
            },
            handleSelectSrcChange: function (v) {
                this.select_host = v
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
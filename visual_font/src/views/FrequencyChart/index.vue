<template>
    <div>
        <card>
            <div  v-if="host_time_rank.length" style="height: 500px">
                <Bar title="最多访问次数的HOST" :series="host_time_rank"></Bar>
            </div>
            <div v-else>
                <span>暂无数据</span>
            </div>
        </card>
        <div style="padding-top: 30px"></div>
        <card>
            <div v-if="host_client_rank.length" style="height: 500px">
                <Bar title="被最多客户端访问的HOST" :series="host_client_rank"></Bar>
            </div>
            <div v-else>
                <span>暂无数据</span>
            </div>
        </card>
        <div style="padding-top: 30px"></div>
        <card>
            <div style="height: 500px"  v-if="client_ranks.length">
                <Bar title="最多请求的客户端" :series="client_ranks"></Bar>
            </div>
            <div v-else>
                <span>暂无数据</span>
            </div>
        </card>
    </div>


</template>

<script>
    import Bar from '@/components/bar'
    import {queryHostClientRank, queryHostTimetRank,queryClient} from 'API'

    export default {
        name: "index",
        components: {Bar},
        data: function () {
            return {
                host_client_rank: [],
                host_time_rank: [],
                client_ranks:[]
            }
        },
        methods: {
            fetchHostClientRank: function () {
                return queryHostClientRank().then((resp) => {
                    this.host_client_rank = resp.data.host_ranks.map((item)=>{
                        return {
                            'name':item['header_value'],
                            'value': item['num']
                        }
                    })
                })
            },
            fetchHostTimeRank: function () {
                return queryHostTimetRank().then((resp) => {

                    this.host_time_rank = resp.data.host_ranks.map((item)=>{
                        return {
                            'name':item['header_value'],
                            'value': item['num']
                        }
                    })
                })
            },
            fetchClientRank: function () {
                return queryClient().then((resp)=>{
                    this.client_ranks = resp.data.client_ranks.map((item)=>{
                        return{
                            'name':item['src'],
                            'value': item['num']
                        }
                    })
                })
            }
        },
        mounted: function () {
            this.fetchHostClientRank();
            this.fetchHostTimeRank();
            this.fetchClientRank();
        }
    }
</script>

<style scoped>

</style>
<template>
    <v-chart :options="option" auto-resize ref="chart"></v-chart>
</template>

<script>
    export default {
        name: "line",
        props: {
            series: Object,
            title: String
        },
        data: function () {
            return {
                option: {
                    tooltip: {
                        trigger: 'axis',
                        axisPointer: {
                            type: 'cross',
                            label: {
                                backgroundColor: '#6a7985'
                            }
                        }
                    },
                    grid: {
                        top: '3%',
                        left: '1.2%',
                        right: '1%',
                        bottom: '3%',
                        containLabel: true
                    },
                    xAxis: [
                        {
                            type: 'time',
                        }
                    ],
                    yAxis: [
                        {
                            type: 'value'
                        }
                    ],
                    series: []
                }
            }
        },
        methods:{
            setCharts:function () {
                let keys = Object.keys(this.series)
                this.option.series = keys.map((item)=>{
                    return {
                        name: item,
                        type: 'line',
                        data: this.series[item]
                    }
                })
            }
        },
        mounted:function(){
            this.setCharts()
        },
        watch: {
            series: {
                handler:function (val, oldVal) {
                    this.setCharts()
                },
                deep:true
            },
            title:{
                handler:function (val, oldVal) {
                    this.setCharts()
                },
            }
        },
    }
</script>

<style>
    .echarts {
        width: 100%;
        height: 100%;
    }
</style>
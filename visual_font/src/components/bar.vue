<template>
    <v-chart :options="option" auto-resize ref="chart"></v-chart>
</template>

<script>
    export default {
        name: "bar",
        props: {
            title: String,
            series: Array,
            color: {type:Array,required: false,}
        },
        data: function () {
            return {
                option: {
                    color: this.color,
                    title: {
                        text: this.text,
                        x: 'center'
                    },
                    tooltip: {
                        trigger: 'axis',
                        axisPointer: {
                            type: 'shadow'
                        }
                    },
                    grid: {
                        left: '3%',
                        right: '4%',
                        bottom: '3%',
                        containLabel: true
                    },
                    legend: {
                        type: 'scroll',
                        orient: 'vertical',
                        right: 10,
                        top: 20,
                        bottom: 20,
                        data: []
                    },
                    xAxis: [
                        {
                            type: 'category',
                            data: [],
                            axisTick: {
                                alignWithLabel: true
                            }
                        }
                    ],
                    yAxis: [
                        {
                            type: 'value'
                        }
                    ],
                    series: [
                        {
                            type: 'bar',
                            barWidth: '60%',
                            data: []
                        }
                    ],
                    dataZoom: [
                        {
                            show: true,
                            start: 0,
                            end: 100
                        }
                    ],
                    animationType: 'scale',
                    animationEasing: 'elasticOut',
                    animationDelay: function (idx) {
                        return Math.random() * 200;
                    }
                }
            }
        },
        methods: {
            setCharts: function () {
                this.option.xAxis[0].data = []
                this.option.series[0].data = []
                this.series.forEach((item)=>{
                    this.option.xAxis[0].data.push(item['name'])
                    this.option.series[0].data.push(item['value'])
                })
                if (!this.color){
                    this.option.color = ['#3398DB']
                } else {
                    this.option.color = this.color
                }
                this.option.title.text = this.title
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
                deep:true
            }
        },
        created() {}
    }
</script>

<style>
    .echarts {
        width: 100%;
        height: 100%;
    }
</style>
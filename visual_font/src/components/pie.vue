<template>
    <v-chart :options="option" auto-resize ref="chart"></v-chart>
</template>

<script>
    export default {
        name: "pie",
        props: {
            title: String,
            series: Array,
            color: {type:Array,required: false,}
        },
        data: function () {
            return {
                option: {
                    title: {
                        text: this.text,
                        x: 'center'
                    },
                    tooltip: {
                        trigger: 'item',
                        formatter: '{a} <br/>{b} : {c} ({d}%)'
                    },
                    series: [
                        {
                            type: 'pie',
                            radius: '55%',
                            center: ['50%', '60%'],
                            data: [],
                            itemStyle: {
                                emphasis: {
                                    shadowBlur: 10,
                                    shadowOffsetX: 0,
                                    shadowColor: 'rgba(0, 0, 0, 0.5)'
                                }
                            }
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
                if (!this.color){
                    this.option.color = ['#3398DB']
                } else {
                    this.option.color = this.color
                }
                this.option.series[0].data = this.series
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
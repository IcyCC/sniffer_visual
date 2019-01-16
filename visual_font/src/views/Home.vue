<style scoped>
  .ivu-layout-header{
    padding: 0px;
    background-color: #ffffff;
  }
  .layout{
    border: 1px solid #d7dde4;
    background: #f5f7f9;
    position: relative;
    border-radius: 4px;
    overflow: scroll;
    height: 100%;
  }
  .content-wrapper{
    padding: 19px;
    min-height: 650px;
    height: ~"calc(100% - 80px)";
    overflow: scroll;
  }
  .layout-logo{
    width: 200px;
    height: 30px;
    font-weight:bold;
    font-size: 20px;
    border-radius: 3px;
    float: left;
    position: relative;
    left: 20px;
    color: #ffffff;
    size: 30px;
  }
  .layout-nav{
    color: #ffffff;
    width: 1200px;
    margin: 0 auto;
    margin-right: 0;
  }
  .layout-footer-center{
    text-align: center;
  }
  .ivu-menu-item{
    font-size: 15px;
    color: #ffffff;
  }
</style>
<template>
  <div class="layout">
    <Layout>
      <Header :style="{position: 'fixed', width: '100%'}">
        <Menu mode="horizontal" theme="primary" :active-name="activity_name" @on-select="onMenuSelect">
          <div class="layout-logo">北林网络流量分析平台</div>
          <div class="layout-nav">
            <MenuItem name="table_show">
              <Icon type="ios-apps" />
                表格数据展示
            </MenuItem>
            <MenuItem name="frequency">
              <Icon type="md-analytics"/>
                访问频率分析
            </MenuItem>
            <MenuItem name="hot_point">
              <Icon type="ios-barcode" />
                兴趣热点分析
            </MenuItem>
          </div>
        </Menu>
      </Header>
      <Content :style="{margin: '88px 20px 0', minHeight: '720px'}">
        <div style="margin-top: 25px">
          <router-view></router-view>
        </div>
      </Content>
      <Footer class="layout-footer-center">2019 &copy; Suchang Yinyue Qidongsheng</Footer>
    </Layout>
  </div>
</template>
<script>
    export default {
        data () {
            return {
                activity_name: "judge"
            }
        },
        methods:{
            onMenuSelect: function(name) {
                this.$router.push({name:name});
            },
            highlightMenu() {
                this.activity_name = this.$route.name
            },
            watch: {
                '$route': {
                    handler: function (){
                        this.highlightMenu()
                    },
                    deep:true
                }
            },
        }
    }
</script>
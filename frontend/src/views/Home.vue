<template>
  <el-container>
    <el-main>
      <el-row align="middle" type="flex">
        <el-col :xs="24" :lg="{span: 6, offset: 9}">
          <div class="temperature-text">{{ temperature }}℃</div>
        </el-col>
      </el-row>

      <el-row align="middle" type="flex">
        <el-col :xs="24" :lg="{span: 6, offset: 9}" align="center">
          <el-progress type="dashboard" :stroke-width="dashboardStrokeWidth" :width="dashboardWidth" :percentage="percentage" :color="colors"></el-progress>
          <p>湿度</p>
        </el-col>
      </el-row>

      <el-row align="middle" type="flex">

        <el-col :xs="24" :lg="{span: 3, offset: 9}">
          <el-button class="big-button" plain style="border-radius: 14px; padding-top: 0px;">
            <h1>气体监测</h1>
            <p>{{ isGas }}</p>
          </el-button>
        </el-col>

        <el-col :xs="24" :lg="{span: 3, offset: 0}">
          <el-button class="big-button" plain style="border-radius: 14px; padding-top: 0px;">
            <h1>火焰监测</h1>
            <p>{{ isFire }}</p>
          </el-button>
        </el-col>
      </el-row>

      <el-row align="middle" type="flex">
        <el-col :xs="24" :lg="{span: 3, offset: 9}">
          <el-button class="big-button" @click="windowFormVisible = true"  plain style="border-radius: 14px; padding-top: 0px;">
            <h1>门窗状态</h1>
            <p>点击查看</p>
          </el-button>
        </el-col>

        <el-col :xs="24" :lg="{span: 3, offset: 0}">
          <el-button class="big-button" @click="lightFormVisible = true" plain style="border-radius: 14px; padding-top: 0px;">
            <h1>室内灯光</h1>
            <p>点击查看</p>
          </el-button>
        </el-col>
      </el-row>

    </el-main>

    <el-dialog title="室内灯光" width="90%" :visible.sync="lightFormVisible">
      <el-table :data="lightData">
        <el-table-column property="name" label="名称" width="150"></el-table-column>
        <el-table-column property="status" label="状态" width="100"></el-table-column>
        <el-table-column label="操作">
          <template slot-scope="scope">
            <el-button
                size="mini"
                @click="switchLight(scope.$index, scope.row)">开/关</el-button>
          </template>
        </el-table-column>
      </el-table>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="lightFormVisible = false">确 定</el-button>
      </div>
    </el-dialog>

    <el-dialog title="门窗状态" width="90%" :visible.sync="windowFormVisible">
      <el-table :data="windowData">
        <el-table-column property="name" label="名称" width="150"></el-table-column>
        <el-table-column property="status" label="状态" width="200"></el-table-column>
      </el-table>
      <div slot="footer" class="dialog-footer">
        <el-button type="primary" @click="windowFormVisible = false">确 定</el-button>
      </div>
    </el-dialog>
  </el-container>




</template>

<script>
// @ is an alias to /src

export default {
  name: 'Home',
  components: {
  },
  data() {
    return {
      dashboardWidth: 240,
      dashboardStrokeWidth: 12,

      timer: null,
      percentage: 0,
      temperature: 0,
      isGas: false,
      isFire: false,

      colors: [
        {color: '#f56c6c', percentage: 20},
        {color: '#e6a23c', percentage: 40},
        {color: '#5cb87a', percentage: 60},
        {color: '#1989fa', percentage: 80},
        {color: '#6f7ad3', percentage: 100}
      ],
      dialogTableVisible: false,
      lightFormVisible: false,
      windowFormVisible: false,


      lightData: [{
        status: 'true',
        name: '讲台灯',
      }, {
        status: 'N/A',
        name: '前排灯',
      },{
        status: 'N/A',
        name: '后排灯',
      }],

      windowData: [{
        status: 'N/A',
        name: '前门',
      }, {
        status: 'N/A',
        name: '后门',
      }],
    };
  },
  mounted() {
    this.queryData();
    this.timer = setInterval(() => {
      setTimeout(this.queryData, 0);
    }, 1500)
  },
  methods: {
    queryData(){
      this.axios
      .get('http://47.104.141.250:9321/data/')
      .then(response => {
        console.log(response.data)
        this.percentage = Number(response.data['humidity']);
        this.temperature = Number(response.data['temperature']);
        this.isGas = Boolean(Number(response.data['is_gas']));
        this.isFire = Boolean(Number(response.data['is_fire']));
      });
      this.axios
      .get('http://47.104.141.250:9321/light/')
      .then(response => {
        if (response.data){
          this.lightData[0].status = '开启';
        } else {
          this.lightData[0].status = '关闭';
        }
      })
    },
    switchLight(index, row){
      this.axios
      .post('http://47.104.141.250:9321/light/')
      .then(response => {
        console.log(response);
      });
      console.log(index, row);
    }
  },
  beforeDestroy(){
    clearInterval(this.timer);
    this.timer = null;
  }
}
</script>

<style>
h1 {
  font-weight: 400;
  color: black;
}

p {
  color: gray;
}

.temperature-text {
  font-size: 120px;
  font-weight: lighter;
  text-align: center;
  margin-bottom: 20px;
}

.el-row {
  margin-bottom: 10px;
}

.el-dialog__wrapper {
  border-radius: 10px;
}

.el-dialog {
  border-radius: 15px;
}

.big-button {
  width: 95%;
  height: 90px;
  padding-top: 0px;
}

</style>
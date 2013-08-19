function draw1(data) {
  "use strict";
  // console.log(data);
  d3.select("#d3-graph")
  .append("ul")
  .selectAll("li")
  .data(data)
  .enter()
  .append("li")
  .text(function(d) {
    return d.name + ': ' + d.status;
  });
    
  d3.selectAll("#d3-graph li")
  .style("font-weight", function(d) {
    return (d.status == 'Good Service') ? 'normal' : 'bold';
  });
}

function draw2(data) {
  var color = d3.scale.category20c();
  d3.select('#d3-graph')
    .append('div')
      .attr("class", "chart")
    .selectAll('.bar')
    .data(data)
    .enter()
    .append('div')
      .attr('class', 'bar')
      .style('margin', '3px')
      .style('outline', '1px solid black')
      .style('width', function(d) {
        return d.count + "px"; 
      })
      .style('background-color', function(d) { 
        return color(d.name); 
      })
      .text(function(d) { return d.name; });
}

function draw3(data) {
  var color = d3.scale.category20c();
  d3.select('#d3-table')
    .selectAll('tbody tr td:last-child')
    .data(data)
      // .style('outline', '2px solid')
      .style('background-color', function(d) { 
        return color(d.name); 
      })
}

function extracTable(table, column) {
  var data = new Array();
  d3.select(table)
    .selectAll(column)
    .call(function(cols) {
      for (i=0; i < cols[0].length; ++i) {
        str = jQuery(cols[0][i]).html();
        str = str.replace(/(^\s+|\s+$)/g, '');
        data.push(parseInt(str));
      }
    });
  return data;
}

function drawHeatmap(table, column) {
  // table = 'div.table-heatmap table';
  // column = 'tbody tr td.stat-percentage';
  cols = extracTable(table, column);
  // console.debug(cols);
  
  // var color = d3.scale.category20c();
  // var color=d3.scale.quantize().domain([0,100]).range(["lightcoral","lightyellow","lightgreen"]);
  // var color=d3.scale.quantize().domain([0,100]).range(d3.scale.category10().range());
  var color=d3.scale.quantize().domain([0,100]).range(d3.range(9));
  d3.select(table)
    .selectAll(column)
    .data(cols)
    .attr('class', function(d) { 
      return "q" + color(d) + "-9"; 
    });
    // .style('background-color', function(d) { 
    //   return color(d); 
    // });
  d3.select(table)
    .selectAll('tbody')
    .attr('class', 'YlOrRd');
  // console.log(cols);
  
  // YlGn
  // YlGnBu
  // GnBu
  // BuGn
  // PuBuGn
  // PuBu
  // BuPu
  // RdPu
  // PuRd
  // OrRd
  // YlOrRd
  // YlOrBr
  // Purples
  // Blues
  // Greens
  // Oranges
  // Reds
  // Greys
}

(function ($) {
  Drupal.behaviors.pharmtrac = {
    attach: function(context, settings) {
      // console.log('d3 integrated');
      // console.log(settings);
      // menu system for javascript
      switch (settings.relativePath) {
        case 'd3':
          draw2(settings.pharmtrac.data);
          draw3(settings.pharmtrac.data);
          break;
        case 'report-heatmap':
          drawHeatmap('div.table-heatmap table', 'tbody tr td.overall-percentage');
          drawHeatmap('div.table-heatmap table', 'tbody tr td.stat-percentage');
          drawHeatmap('div.table-heatmap table', 'tbody tr td.routine-percentage');
          break;
        case 'calendar':
          break;
      }
    }
  }
})(jQuery);

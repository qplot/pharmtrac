var d3_settings = {
  distance: 5.5,
  charge: 5
};

jQuery(document).ready(function($) {
  $('#slider-distance').slider({
    change: function(event, ui) {
      d3_settings.distance = ui.value;
    }
  });
  $('#slider-charge').slider({
    change: function(event, ui) {
      d3_settings.charge = ui.value;
    }
  });
    
  var graph = {
    "nodes":[
      {"name":"Device","group":1,"size":5},                     // 0
      {"name":"Dispense","group":1,"size":10},                  // 1
      {"name":"Location","group":1,"size":8},                   // 2
      {"name":"NDC","group":1,"size":5},                        // 3
      {"name":"Med","group":1,"size":8},                        // 4
      {"name":"Order","group":1,"size":10},                     // 5
      {"name":"Patient","group":1,"size":5},                    // 6
      {"name":"Request","group":1,"size":8},                    // 7
      {"name":"Room","group":1,"size":5},                       // 8
      {"name":"Shift","group":1,"size":5},                      // 9
      {"name":"User","group":1,"size":8},                       // 10
      {"name":"Wallboard","group":1,"size":8},                  // 11
      {"name":"Color","group":1,"size":3},                      // 12
      {"name":"Location Property","group":1,"size":5},          // 13
      {"name":"Med Feature","group":1,"size":3},                // 14
      {"name":"Mobile Feature","group":1,"size":3},             // 15
      {"name":"Mobile Role","group":1,"size":3},                // 16
      {"name":"Priority Level","group":1,"size":3},             // 17
      {"name":"Reject Type","group":1,"size":3},                // 18
      {"name":"Request Type","group":1,"size":3},               // 19
      {"name":"Column","group":1,"size":3},                     // 20
      {"name":"Picture","group":1,"size":3},                    // 21
      {"name":"Message","group":1,"size":5},                    // 22
      {"name":"Footer Color","group":1,"size":3},               // 23
    ],
    "links":[
      {"source":0,"target":7,"value":9},
      {"source":1,"target":7,"value":9},
      {"source":1,"target":5,"value":9},
      {"source":1,"target":11,"value":9},
      {"source":1,"target":8,"value":3},
      {"source":3,"target":4,"value":1},
      {"source":4,"target":5,"value":1},
      {"source":5,"target":6,"value":1},
      {"source":2,"target":8,"value":1},
      {"source":2,"target":11,"value":1},
      {"source":10,"target":2,"value":1},
      {"source":10,"target":16,"value":1},
      {"source":11,"target":12,"value":1},
      {"source":11,"target":23,"value":1},
      {"source":13,"target":2,"value":1},
      {"source":14,"target":4,"value":1},
      {"source":15,"target":16,"value":1},
      {"source":17,"target":1,"value":1},
      {"source":18,"target":7,"value":1},
      {"source":19,"target":7,"value":1},
      {"source":20,"target":11,"value":1},
      {"source":21,"target":7,"value":1},
      {"source":4,"target":19,"value":1},
      {"source":7,"target":10,"value":1},
      {"source":7,"target":2,"value":9},
    ]
  };

  var width = 800, height = 600;
  var color = d3.scale.category20();

  // define force field
  var force = d3.layout.force()
    .charge(-350)
    .linkDistance(100)
    .size([width, height]);

  // add svg graph
  var svg = d3.select("div#graph")
    .append("svg")
    .attr("width", width)
    .attr("height", height);

  // add nodes and links 
  // turn on force field
  force.nodes(graph.nodes)
    .links(graph.links)
    // .linkDistance(function(d) {
    //   return d.source.size * d.target.size * d3_settings.distance;
    // })
    // .charge(function(d) {
    //   return - d.size * 10;
    // })
  
  var link = svg.selectAll(".link")
    .data(graph.links);
    // .enter()
    // .append("line")
    // .attr("class", "link")
    // .style("stroke-width", function(d) {
    //   return Math.sqrt(d.value);
    // });

  var node = svg.selectAll(".node")
    .data(graph.nodes)
    .enter()
    .append("g")
    .attr("class", "node")
    .call(force.drag);
  
  node.append("circle")
    .attr("class", "circle")
    .attr("r", function(d) { return d.size; })
    .style("fill", function(d) { return color(d.group); });
    
  node.append("text")
    .text(function(d) { return d.name; })
    .attr("font-size", function(d) { return d.size+8; })
    .attr("transform", function(d) { 
      return "translate(" + d.size + ",0)";
    });
    
  force.start();
  
  force.on("tick", function() {
    link.attr("x1", function(d) { return d.source.x; })
        .attr("y1", function(d) { return d.source.y; })
        .attr("x2", function(d) { return d.target.x; })
        .attr("y2", function(d) { return d.target.y; });

    node.attr("transform", function(d) { 
      return "translate(" + d.x + "," + d.y + ")";
    });
    
    force.linkDistance(function(d) {
        return d.source.size * d.target.size * d3_settings.distance / 20;
      })
    .charge(function(d) {
      return - d.size * d3_settings.charge;
    })
    .start();
  });  
  
});


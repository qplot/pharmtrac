(function ($) {
  Drupal.behaviors.pharmtrac = {
    attach: function(context, settings) { 
      
      // console.debug('client start')
      var socket = io.connect('http://localhost:3000');
      
      socket.on('connect', function() {
        console.debug('client connected');
        // document.getElementById('nodejs').innerHTML = '123';
      });
      socket.on('text', function(obj,msg) {
        console.debug(msg);
        document.getElementById('nodejs').innerHTML = msg;
      });
      
    }
  }
})(jQuery);

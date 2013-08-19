(function ($) {
  Drupal.behaviors.pharmtrac = {
    attach: function(context, settings) { 

      Pusher.log = function(message) {
        if (window.console && window.console.log) window.console.log(message);
      };

      WEB_SOCKET_DEBUG = true;

      var pusher = new Pusher('02807555460b0eac48d9');
      var channel = pusher.subscribe('test_channel');
      var status = 'on';
      channel.bind('my_event', function(data) {
        $('#heartbeat').append(data + '<br />');
        if (status == 'on') 
          status = 'off';
        else
          status = 'on';
        $.get('http://localhost:8934/blink1/' + status, function(data) {
        });
          // window.location = '' + data;  
      });
    }  
  }
})(jQuery);

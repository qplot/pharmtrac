!function($){
  $(function () {
    $('header a[rel=tooltip]').tooltip({
      placement : 'bottom'   }     
    );
  
    $('a[rel=tooltip]').tooltip();

    $('a[rel=popover]').popover();
  });
}(window.jQuery)
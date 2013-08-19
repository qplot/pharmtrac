function RequestListCtrl($scope, $http) {
  // list items
  $scope.indexItems = function() {
    $scope.items = {};
    $http.get('http://localhost/pharmtrac/api?command=request&picture=.')
      .success(function(data) {
        $scope.items = data.data;
        
        for (var i=0; i < $scope.items.length; i++) {
          var uri = $scope.items[i].picture.replace("private://", "http://localhost/pharmtrac/system/files/");
          $scope.items[i].picture = uri;
        }
        $scope.selectItem(0);
        // console.debug($scope.items);
      }
    );
  }
  
  $scope.selectItem = function(index) {
    if ((index >= 0) && (index < $scope.items.length))
      $scope.selected = index;
    else
      $scope.selected = -1;
  }

  $scope.items = {};
  $scope.selected = -1;
  $scope.indexItems();
}
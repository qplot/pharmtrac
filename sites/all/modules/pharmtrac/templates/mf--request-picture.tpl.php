<?php
  $path = drupal_get_path('module', 'pharmtrac');
  
  drupal_add_js('http://ajax.googleapis.com/ajax/libs/angularjs/1.0.5/angular.min.js', 'external');
  drupal_add_js($path . '/js/request_picture.js');
?>

<div class="<?php print $classes; ?>">

  <div class="row-fluid" ng-controller="RequestListCtrl">
    <div class="span4">
      
      <!-- Modal for add -->
      <div class="modal hide" id="addModal">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
          <h3>{{op}} Location <em>{{name}}</em></h3>
        </div>
        <div class="modal-body">

          <div class="control-group">
            <label class="control-label" for="inputName">Name</label>
            <div class="controls">
              <input ng-model="name" type="text" placeholder="Location name" required>
            </div>
          </div>
          <div class="control-group">
            <label class="control-label" for="inputLocation">Location</label>
            <div class="controls">
              <textarea ng-model="location" placeholder="Location or description" class="span12"></textarea>
            </div>
          </div>
          <div class="control-group">
            <label class="control-label" for="inputPosition">Position</label>
            <div class="controls">
              <input ng-model="latitude" placeholder="Latitude" class="span4">
              <input ng-model="longitude" placeholder="Longitude" class="span4">
            </div>
          </div>
        </div>
        <div class="modal-footer">
          <button ng-click='addUpdateItem(selected)' class="btn btn-primary" data-dismiss="modal">{{op}}</button>
          <a href="#" class="btn close" data-dismiss="modal">Close</a>
        </div>
      </div>
              
      <!-- Modal for delete -->
      <div class="modal hide" id="deleteModal">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
          <h3>Delete {{name}} ?</h3>
        </div>
        <div class="modal-body">
          Are you sure you want to delete location <em>{{name}}</em> ?
        </div>
        <div class="modal-footer">
          <button ng-click='deleteItem(selected)' class="btn btn-primary" data-dismiss="modal">Delete Location</button>
          <a href="#" class="btn close" data-dismiss="modal">Close</a>
        </div>
      </div>
      
      <!--  -->
      <div class="">
        <div class="clearfix">
          <div class="btn-group pull-right">
          </div>
          <!-- Total: {{items.length}} -->
        </div>
        <div class="input-append">
          <input ng-model="query" type="text" class="input">
          <button class="btn" type="button">Search</button>
          <button ng-click='indexItems()' class="btn"><i class="icon-refresh"></i> </button>
        </div>
        
        <br />
        <ul class="nav nav-tabs nav-stacked">
          <li class="nav-header"> Picture Lists </li>
          <li ng-repeat="item in items | filter: query" class="{{ {true: 'active', false: ''}[$index == selected] }}">
            <!-- <div class="pull-right">
            <a ng-click='fillItem($index);' type="button" data-toggle="modal" data-target="#addModal" class="btn-link" title="edit"><i class="icon-edit"></i></a>
            <a ng-click='fillItem($index)' type="button" data-toggle="modal" data-target="#deleteModal" class="btn-link" title="delete"><i class="icon-remove"></i></a>
            </div> -->
            <a ng-click="selectItem($index)">{{item.dispense}}</a>
            <!-- <small>{{item.note}}</small> -->
            <!-- <small>{{item.picture}}</small> -->
          </li>
        </ul>
      </div>  
    </div>
    
    <div class="span8">
      <div ng-show="selected+1">
        <h1>{{ items[selected].dispense }}</h1>
        <p>submitted user : {{items[selected].user}} </p>
        <p>sumbitted device : {{items[selected].device}} </p>
        <!-- <p>{{ items[selected].picture }}</p> -->
        <p>
        <img ng-src="{{items[selected].picture}}">
        </p>
      </div>
      <div ng-hide="selected+1">
        <h1>No record</h1>
      </div>
    </div>
  </div>
</div>

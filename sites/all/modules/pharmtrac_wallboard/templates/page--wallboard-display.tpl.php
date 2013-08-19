<?php if (!empty($page['navigation'])): ?>
  <?php //if (!empty($breadcrumb)): print $breadcrumb; endif;?>
<?php endif; ?>
<?php if (!empty($page['content_full'])): ?>
  <div class="main-container full-container">
  <?php print render($page['content_full']); ?>
  </div> 
<?php endif; ?>
<?php if (!empty($page['footer_bottom'])): ?>
  <footer class="footer_bottom">
  <?php print render($page['footer_bottom']); ?>
  </footer> 
<?php endif; ?>

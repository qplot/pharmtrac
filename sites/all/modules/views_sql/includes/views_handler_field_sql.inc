<?php
/**
 * @file
 * Field handler which allows to add SQL line
 */

/**
 * Field handler which allows to add SQL functions
 */
class views_handler_field_sql extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['sql_tables'] = array('default' => '');
    $options['sql_function'] = array('default' => '(1+1)');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['sql_tables'] = array(
      '#type' => 'textarea',
      '#title' => t('SQL Tables'),
      '#default_value' => $this->options['sql_tables'],
      '#description' => t('List tables that you want to include.'),
      '#rows' => 3,
    );
    $form['sql_function'] = array(
      '#type' => 'textarea',
      '#title' => t('SQL Editor'),
      '#default_value' => $this->options['sql_function'],
      '#description' => t('Custom field function written in SQL.'),
      '#rows' => 5,
      '#resizable' => TRUE,
    );

    parent::options_form($form, $form_state);
  }

  function query() {
    // field_alias records field alias 
    foreach (explode(',',$this->options['sql_tables']) as $value) {
      $this->view->query->add_table($value);
    }
    $this->field_alias = $this->view->query->add_field(NULL, $this->options['sql_function'], "vhfs");
  }

  function render($values) {
    return parent::render($values);
  }  

}

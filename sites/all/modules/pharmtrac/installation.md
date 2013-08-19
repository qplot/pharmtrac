Pharmtrac Installation
====

Server Software
----
* **`svn`**
* **`drush`**

Pharmtrac Installation
----
Pharmtrac comes with pre-packaged code, including `Drupal` core, third party modules and `pharmtrac` module. Since we have some fixes on certain module (or core), we should use this package instead of downloading them separately. 

To get the package, use svn, use proper release tag (ex. trunk)  

    svn co http://fang@suber.plusdeltaapps.com:9880/Pharmtrac-Drupal/trunk duke

To install drupal from scratch, we can run a drush command,  

    sudo drush si --db-url=mysql://root:root@localhost/pharmtracpd_duke --site-name=Duke --account-pass=admin

This will create pharmtracpd_duke database with default user admin/admin, to check if it works, we can just go to visit URL, ex. `http://50.112.243.170/duke`. By default it installs drupal as a single site, for multi-site installation, please refer to `Server Settings` section of this document.

Enable `pharmtrac` module and resolve dependency by clicking yes to enable them along with `pharmtrac` module   

    drush en pharmtrac

Clear cache afterwards to reflect new changes,  
    
    drush cc all

Make file folder writable  

    sudo chmod -R ogu+w files

Pharmtrac Content
----
Pharmtrac uses node_export to transfer node, in order to add initial pharmtrac content, use

    drush fr pharmtrac_content

Due to the design of node_export, you need to delete all imported contents before installing `pharmtrac_content`

Pharmtrac Shortcut
----
Several shortcut settings just to make operating Drupal easier,

    drush dis overlay
    drush en admin_menu_toolbar
    drush dis toolbar

Pharmtrac Setting 
----
Pharmtrac comes with some of the default settings, we need to check and adjust some of them based on the hospital preference.

**On page `/request-types`,**   
+ Make sure the order of request type to be in the correct order, ex. `received`, `print` etc. ;

**On page `/settings`,**  
+ Set `application key` to be `plusdelta123`;  
+ Set `require password authentication` to be `No`;

**On `/settings/workflow`,**  
+ Set the default sequence to be `recieved`, `print`, `prepare`, `verify`, `pickup`, `dropoff`, `done`;  
+ Set  the next request type after reject, ex. `print`  
+ Select special rules from `Tube`, `Regigerator`, `Dropoff Location`, `Allow to transfer`, `Dispense creation`, `Hand delivery`;  
+ Assign a default order node `id` to match the id of order `ORDER`;  
+ Assign a default dispense node `id` to match the id of dispense `DISPENSE`;

**On page `/users`,**
+ Add a user with barcode `SE23-231`


The above are currently all mandatory steps to make pharmtrac functional. 

Update Pharmtrac
----
At the time we have a existing system running, we can update it to the latest version by first pulling new code changes from the git repository, feature reverting the current installed database accordingly to the new code, and lastly clearing the cache to reflect to new changes.  
    
    svn up
    drush fr pharmtrac
    drush cc all

Each of the step above are essential for module deployment, and might take 1-10 minutes, so be patient.

Server Setup
----

To get multi-site setup, we need to clone one of the settings folder. By default pharmtrac comes with two profile, mockup and pharmtrac. 

Clone any one of the profile to proper folder, ex.  
    
    cp -R sites/localhost.mockup sites/50.112.243.170.duke

If duke drupal instance is not reachable directly from http://50.112.243.170, then a soft link needs to be created, ex.  
    
    ln -s /(my drupal root folder) var/www/duke

Pharmtrac Theme
----
By default pharmtrac doesn't require any theme to be installed other than Drupal default theme. To make it look better, we can turn on custom theme and adjust some of the block layout. Two base themes has been tested for pharmtrac, Acquia Marina and Bootstrap.

### Enable Mockup (based on Bootstrap)
    
    drush en mockup
    drush vset theme_default mockup
    drush cc all

### Adjust blocks
+ Add main menu block to Primary region, remove others
+ Add footer message block to Footer region, remove others

### Wallboard contexts
+ Add `full_width` context
+ Add `wallboard` context 
    + setup path to be `wallboard-dispenses/*`, 
    + disable `Navigation`, `Primary`, `Secondary`, `Footer` on `mockup` theme, 
    + drag `Pharmtrac Wallboard Statistics` to region `footer_bottom` (sticky footer)

### Adjust main menu
+ Add `Contents` <nolink> before `Bundle` and a <separator> on the above
+ Add `Types` <nolink> before `Color` and a <separator> on the above
+ Add `Reports` <nolink> before `Ontime` and a <separator> on the above
+ Add `Extras` <nolink> before `Content type` and a <separator> on the above

Testing Case
----
### Test adding a order (or HL7)
+ under `/messages`, process the message with ID `HL7`
+ a new order `01234560` will appear under `\orders`

### Test adding a dispense (or PS)
+ under `/messages`, process the message with ID `PS`
+ a new dispense `JZ01234560-ABCD` will be added under `\dispenses`

### Test adding requests 
+ open `Dev HTTP Client` or similar http app for your browser
+ create a request thourgh HTTP post operation   
    + URL: `/api?command=createRequest`
    + Method: `POST`
    + Header: `Content-Type`, `application/x-www-form-urlencoded`
    + Body: override=0&barcode=JZ01234560-ABCD&user=SE23-231&location=2000&type=received&device=DE333&ndc=3245453&note=123
    

Trouble Shooting
----
A: If there's a bug preventing you installing pharmtrac module correctly, you can always roll back to refresh install of Drupal,  

    sudo drush si --site-name=Duke --account-pass=admin
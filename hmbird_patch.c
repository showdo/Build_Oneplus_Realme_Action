         #include <linux/init.h>
         #include <linux/module.h>
         #include <linux/of.h>
         #include <linux/slab.h>
         #include <linux/string.h>

         static int __init hmbird_patch_init(void)
         {
             struct device_node *ver_np;
             const char *type;
             int ret;

             ver_np = of_find_node_by_path("/soc/oplus,hmbird/version_type");
             if (!ver_np) {
                  pr_info("hmbird_patch: version_type node not found\n");
                  return 0;
             }

             ret = of_property_read_string(ver_np, "type", &type);
             if (ret) {
                  pr_info("hmbird_patch: type property not found\n");
                  of_node_put(ver_np);
                  return 0;
             }

             if (strcmp(type, "HMBIRD_OGKI")) {
                  of_node_put(ver_np);
                  return 0;
             }

             struct property *prop = of_find_property(ver_np, "type", NULL);
             if (prop) {
                  struct property *new_prop = kmalloc(sizeof(*prop), GFP_KERNEL);
                  if (!new_prop) {
                       pr_info("hmbird_patch: kmalloc for new_prop failed\n");
                       of_node_put(ver_np);
                       return 0;
             }
             memcpy(new_prop, prop, sizeof(*prop));
             new_prop->value = kmalloc(strlen("HMBIRD_GKI") + 1, GFP_KERNEL);
             if (!new_prop->value) {
                  pr_info("hmbird_patch: kmalloc for new_prop->value failed\n");
                  kfree(new_prop);
                  of_node_put(ver_np);
                  return 0;
            }
            strcpy(new_prop->value, "HMBIRD_GKI");
            new_prop->length = strlen("HMBIRD_GKI") + 1;

            if (of_remove_property(ver_np, prop) != 0) {
                 pr_info("hmbird_patch: of_remove_property failed\n");
                 return 0;
            }
            if (of_add_property(ver_np, new_prop) !=0) {
                 pr_info("hmbird_patch: of_add_property failed\n");
                 return 0;
            }
            pr_info("hmbird_patch: success from HMBIRD_OGKI to HMBIRD_GKI\n");
         }
         else {
             pr_info("hmbird_patch: type property structure not found\n");
         }
         of_node_put(ver_np);
         return 0;
         }
         early_initcall(hmbird_patch_init);
         MODULE_LICENSE("GPL");
         MODULE_AUTHOR("reigadegr");
         MODULE_DESCRIPTION("Forcefully convert HMBIRD_OGKI to HMBIRD_GKI.");

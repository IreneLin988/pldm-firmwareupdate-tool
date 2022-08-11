
uint8_t str_type(const char *input);
uint32_t update_opt_val(const char *input);
int init_des_type(const char *input, uint16_t *output);
int hexstr_tobin(char *input, uint8_t len, uint8_t *output, uint8_t size);
int str_tobin(char *input, uint8_t len, uint8_t *output, uint8_t size);
uint16_t compo_class_val(const char *input);
uint16_t compo_opt_val(const char *input);
uint16_t act_meth_val(const char *input, uint16_t bit_val);
uint8_t *timestamp104(void);
int power(int num);
int bit32_ctrl_1 (uint32_t *input, int bit);
int bit16_ctrl_1 (uint16_t *input, int bit);
int bit8_ctrl_1 (uint8_t *input, int bit);
int GetMsbPos(int num);

#ifndef NSK_ROMTYPE_TRAINER
#define NSK_ROMTYPE_TRAINER

/*!
 * \brief  List of trainer presence posibilities
 *
 * \note Constructed from xconst_trainer file via XMacro
 */
enum nsk_trainer_type {

#   define X(key, name, value, description) \
    NSK_TRAINER_ ## name = value,

#       include <xconstants/nsk_xconst_trainer.x>

#   undef X

};

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The trainer type
 * \return Static string
 */
const char *nsk_trainer_name(enum nsk_trainer_type type);

#endif
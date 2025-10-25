#ifndef NSK_ROMTYPE_MIRRORING
#define NSK_ROMTYPE_MIRRORING

/*!
 * \brief  Possible nametables mirrorings
 */
enum nsk_mirroring_type {
    NSK_MIRRORING_HORIZONTAL,
    NSK_MIRRORING_VERTICAL
};

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The mirroring type
 * \return Static string
 */
const char *nsk_mirroring_name(enum nsk_mirroring_type type);

#endif
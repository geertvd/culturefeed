<?php

/**
 * @file
 * Contains privacy_settings.inc.
 */

/**
 * Returns the options for the user name based on privacy settings.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 *
 * @return array
 *   The display options.
 */
function culturefeed_ui_privacy_settings_get_options(CultureFeed_User $culturefeed_user) {

  $options = array(
    'age_location', 'location', 'name', 'nick',
  );

  // Only show the options that have all values.
  $display_options = array();
  foreach ($options as $option) {
    $display = culturefeed_ui_privacy_settings_get_display_name($culturefeed_user, $option, FALSE);
    if ($display) {
      $display_options[$option] = $display;
    }
  }

  // Add in anonymous.
  $display_options['anonymous'] = t('Anonymous');

  return $display_options;

}

/**
 * Returns the current setting for the user name based on privacy settings.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 *
 * @return string
 *   The current setting.
 */
function culturefeed_ui_privacy_settings_get_current_setting(CultureFeed_User $culturefeed_user) {

  $privacy_config = $culturefeed_user->privacyConfig;

  // Default.
  $privacy_setting = 'anonymous';

  if (
    $privacy_config->givenName == 'public' &&
    $privacy_config->dob == 'public' &&
    $privacy_config->homeAddress == 'public'
  ) {
    $privacy_setting = 'age_location';
  }

  elseif (
    $privacy_config->givenName == 'public' &&
    $privacy_config->homeAddress == 'public'
  ) {
    $privacy_setting = 'location';
  }

  elseif ($privacy_config->givenName == 'public') {
    $privacy_setting = 'name';
  }

  elseif ($privacy_config->nick == 'public') {
    $privacy_setting = 'nick';
  }

  return $privacy_setting;

}

/**
 * Returns the username based on privacy settings.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 * @param string $setting
 *   The selected setting (only needed for form options).
 * @param bool $use_fallback
 *   Use fallback for missing data (only needed for actual display).
 *
 * @return string
 *   The username.
 */
function culturefeed_ui_privacy_settings_get_display_name(
  CultureFeed_User $culturefeed_user,
  $setting = NULL,
  $use_fallback = TRUE
) {

  if (!isset($setting)) {
    $setting = culturefeed_ui_privacy_settings_get_current_setting($culturefeed_user);
  }

  // If no first name is available all options except nick are not available.
  if ($culturefeed_user->givenName) {

    // Age location.
    if ($setting == 'age_location') {

      $display = array();

      // Name.
      $name = $culturefeed_user->givenName;
      if ($culturefeed_user->familyName) {
        $name .= ' ' . $culturefeed_user->familyName;
      }
      $display[] = $name;

      // Age.
      if ($culturefeed_user->dob) {

        $dob = date_create();
        $dob->setTimestamp($culturefeed_user->dob);
        $today = date_create();
        $interval = date_diff($dob, $today);
        $years = $interval->format('%y');
        $display[] = t('!years years', array('!years' => $years));

      }

      // City.
      if ($culturefeed_user->city) {
        $display[] = $culturefeed_user->city;
      }

      // Return the display.
      if (count($display) == 3) {
        return implode(', ', $display);
      }

    }

    // City.
    if ($setting == 'location') {

      // Name.
      $display = array($culturefeed_user->givenName);

      // City.
      if ($culturefeed_user->city) {
        $display[] = $culturefeed_user->city;
      }

      // Return the display.
      if (count($display) == 2) {
        return implode(', ', $display);
      }

    }

    // Name.
    if ($setting == 'name') {
      return $culturefeed_user->givenName;
    }

  }

  // Nick.
  if ($setting == 'nick') {
    return $culturefeed_user->nick;
  }

  // Fallback.
  return ($use_fallback) ? $culturefeed_user->nick : '';

}

/**
 * Sets the privacy settings based on setting.
 *
 * @param CultureFeed_UserPrivacyConfig $privacy_config
 *   The privacy config.
 * @param string $setting
 *   The selected setting (only needed for form options).
 */
function culturefeed_ui_privacy_settings_set_options(CultureFeed_UserPrivacyConfig &$privacy_config, $setting) {

  // Defaults.
  $privacy_config->gender = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
  $privacy_config->bio = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

  if ($setting == 'age_location') {

    $privacy_config->givenName = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->familyName = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->nick = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;

  }

  if ($setting == 'location') {

    $privacy_config->givenName = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->familyName = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->nick = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;

  }

  if ($setting == 'name') {

    $privacy_config->givenName = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
    $privacy_config->familyName = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->nick = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;

  }

  if ($setting == 'nick') {

    $privacy_config->givenName = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->familyName = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->nick = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

  }

  if ($setting == 'anonymous') {

    $privacy_config->givenName = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->familyName = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->nick = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
    $privacy_config->bio = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;

  }

}
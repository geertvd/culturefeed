<?php

/**
 * @file
 * Contains Drupal\culturefeed\Controller\EventsController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultuurNet\UDB3\Event\EventTaggerServiceInterface;
use Symfony\Component\HttpFoundation\Request;
use CultuurNet\UDB3\UsedKeywordsMemory\DefaultUsedKeywordsMemoryService;
use CultureFeed_User;
use CultuurNet\UDB3\Symfony\JsonLdResponse;

class EventsController extends ControllerBase {

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed_udb3.event.tagger'),
      $container->get('culturefeed_udb3.event.used_keywords_memory'),
      $container->get('culturefeed.current_user')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param EventTaggerServiceInterface $event_tagger
   *   The event tagger.
   * @param DefaultUsedKeywordsMemoryService $used_keywords_memory
   *   The event tagger.
   * @param CultureFeed_User $user
   *   The event tagger.
   */
  public function __construct(
    EventTaggerServiceInterface $event_tagger,
    DefaultUsedKeywordsMemoryService $used_keywords_memory,
    CultureFeed_User $user
  ) {
    $this->eventTagger = $event_tagger;
    $this->usedKeywordsMemory = $used_keywords_memory;
    $this->user = $user;
  }

  /**
   * Tag culturefeed events.
   *
   * @param Request $request
   *   The request.
   *
   * @return JsonLdResponse
   *   A json response.
   */
  public function tag(Request $request) {

    $keyword = $request->query->get('keyword');
    $event_ids = $request->query->get('events');

    $response = JsonLdResponse::create();

    try {

      $command_id = $this->eventTagger->tagEventsById($event_ids, $keyword);

      $user = $this->user;
      $this->usedKeywordsMemory->rememberKeywordUsed(
        $user->id,
        $keyword
      );

      $response->setData(['commandId' => $command_id]);

    }
    catch (\Exception $e) {

      $response->setStatusCode(400);
      $response->setData(['error' => $e->getMessage()]);

    };

    return $response;

  }

}
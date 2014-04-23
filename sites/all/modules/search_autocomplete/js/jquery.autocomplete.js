/**
 * @file
 * SEARCH AUTOCOMPLETE (version 7.3-x)
 */
 
/*
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */
 
(function ($) {
  Drupal.behaviors.search_autocomplete = {
    attach: function(context) {
      if (Drupal.settings.search_autocomplete) {
        $.each(Drupal.settings.search_autocomplete, function(key, value) {
          var NoResultsLabel = Drupal.settings.search_autocomplete[key].no_results;
          $(Drupal.settings.search_autocomplete[key].selector).addClass('ui-autocomplete-processed').autocomplete({
            minLength: Drupal.settings.search_autocomplete[key].minChars,
            source: function(request, response) {
              if (Drupal.settings.search_autocomplete[key].type == 0) {             // external URL
                $.getJSON(Drupal.settings.search_autocomplete[key].datas, { q: request.term }, function (results) {
                  // Only return the number of values set in the settings.
                  if (!results.length && NoResultsLabel) {
                      results = [NoResultsLabel];
                  }   
                  response(results.slice(0, Drupal.settings.search_autocomplete[key].max_sug));
                });
              } else if (Drupal.settings.search_autocomplete[key].type == 1) {      // internal URL
                $.getJSON(Drupal.settings.search_autocomplete[key].datas + request.term, { }, function (results) {
                  // Only return the number of values set in the settings.
                  if (!results.length && NoResultsLabel) {
                      results = [NoResultsLabel];
                  }
                  response(results.slice(0, Drupal.settings.search_autocomplete[key].max_sug));
                });
              } else if (Drupal.settings.search_autocomplete[key].type == 2) {      // static resources
                var results = $.ui.autocomplete.filter( Drupal.settings.search_autocomplete[key].datas, request.term );
                    if (!results.length && NoResultsLabel) {
                    results = [NoResultsLabel];
                }
                // Only return the number of values set in the settings.
                response(results.slice(0, Drupal.settings.search_autocomplete[key].max_sug));
              }
            },
            open: function(event, ui) {
              $(".ui-autocomplete li.ui-menu-item:odd").addClass("ui-menu-item-odd");
              $(".ui-autocomplete li.ui-menu-item:even").addClass("ui-menu-item-even");
            },
            select: function(event, ui) { 
              if (ui.item.label === NoResultsLabel) {
                event.preventDefault();
              } else
              if (Drupal.settings.search_autocomplete[key].auto_redirect == 1 && ui.item.link) {
                document.location.href = ui.item.link;
              } else if (Drupal.settings.search_autocomplete[key].auto_submit == 1) {
                  $(this).val(ui.item.label);
                  $(this).closest("form").submit();
              }
            },
            focus: function (event, ui) {
              if (ui.item.label === NoResultsLabel) {
                  event.preventDefault();
              }
            }
          }).autocomplete("widget").attr("id", "ui-theme-" + Drupal.settings.search_autocomplete[key].theme);
        });
      }
    }
  };
})(jQuery);